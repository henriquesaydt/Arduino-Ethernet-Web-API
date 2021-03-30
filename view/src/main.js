import '@babel/polyfill'
import 'mutationobserver-shim'
import Vue from 'vue'
import './plugins/bootstrap-vue'
import App from './App.vue'
import Vuex from 'vuex'
import axios from 'axios'

import { library } from '@fortawesome/fontawesome-svg-core'
import { faLightbulb } from '@fortawesome/free-solid-svg-icons'
import { faThermometerHalf } from '@fortawesome/free-solid-svg-icons'
import { faMicrochip } from '@fortawesome/free-solid-svg-icons'
import { faUpload } from '@fortawesome/free-solid-svg-icons'
import { faBars } from '@fortawesome/free-solid-svg-icons'
import { faCircle } from '@fortawesome/free-solid-svg-icons'
import { faSync } from '@fortawesome/free-solid-svg-icons'
import { FontAwesomeIcon } from '@fortawesome/vue-fontawesome'

import { BVToastPlugin } from 'bootstrap-vue'

library.add(faLightbulb)
library.add(faThermometerHalf)
library.add(faMicrochip)
library.add(faUpload)
library.add(faBars)
library.add(faCircle)
library.add(faSync)

Vue.use(Vuex)
Vue.use(BVToastPlugin)

Vue.component('font-awesome-icon', FontAwesomeIcon)

Vue.config.productionTip = false

//Status ID - 0: Desconectado - 1: Conectando - 2: Conectado

const store = new Vuex.Store({
    state: {
        config: {
            ip: "",
            port: "",
            token: ""
        },
        status: {id: 0, text: "Desconectado", color: "red", error: ""},
        gettingData: false,
        sensorsData: {
            lm35: "",
            ldr: "",
            hcsr04: ""
        }
    },
    getters: {
        status: state => {
            return state.status;
        },
        config: state => {
            return state.config;
        },
        gettingData: state => {
            return state.gettingData;
        },
        sensorsData: state => {
            return state.sensorsData;
        }
    },
    mutations: {
        connect(state, {ip, port, token}) {
            state.status = {id: 1, text: "Conectando", color: "#d69d00", error: ""};
            if (!ip || !port || !token) {
                state.status = {id: 0, text: "Desconectado", color: "red", error: "Informações de conexão inválidas"};
                return;
            }
            state.config = {
                ip: ip,
                port: port,
                token: token
            }
            axios({
                url: `http://${state.config.ip}:${state.config.port}`,
                method: 'post',
                data: {token: token},
                headers: {'Content-Type': 'text/plain'}
            })
            .catch( error => {
                console.log(error);
                state.status = {id: 0, text: "Desconectado", color: "red", error: "Não foi possível se comunicar com o Arduino"};
            })
            .then( response => {
                if (response.data.status == "success") {
                    state.status = {id: 2, text: "Conectado", color: "green"};
                }
                else {
                    state.status = {id: 0, text: "Desconectado", color: "red", error: response.data.message};
                }
            })
        },
        desconnect(state) {
            state.config = {
                ip: "",
                port: "",
                token: ""
            };
            state.status = {id: 0, text: "Desconectado", color: "red", error: ""}
        },
        sendEntries(state, {data, reference}) {
            axios({
                url: `http://${state.config.ip}:${state.config.port}`,
                method: "post",
                data: data,
                headers: {'Content-Type': 'text/plain'}
            })
            .catch(function (error) {
                console.log(error);
                state.status = {id: 0, text: "Desconectado", color: "red", error: ""};
                reference.$bvToast.toast('Não foi possível enviar os dados, nenhuma resposta do Arduino recebida. Verifique a conexão e tente novamente', {
                    title: "Falha ao enviar dados",
                    variant: "danger",
                    solid: true
                })
            })
            .then((response) => {
                console.log(response.data);
            })
        },
        getData(state, {reference}) {
            state.gettingData = true;
            axios({
                url: `http://${state.config.ip}:${state.config.port}`,
                method: "post",
                data: {token: state.config.token, receiveData: true},
                headers: {'Content-Type': 'text/plain'}
            })
            .catch(function (error) {
                state.gettingData = false;
                console.log(error);
                state.status = {id: 0, text: "Desconectado", color: "red", error: ""};
                reference.$bvToast.toast('Não foi possível receber dados dos sensores, nenhuma resposta do Arduino recebida. Verifique a conexão e tente novamente', {
                    title: "Falha ao receber dados",
                    variant: "danger",
                    solid: true
                });
            })
            .then((response) => {
                console.log(response.data);
                state.gettingData = false;
                state.sensorsData = {
                    lm35: response.data.dados,
                    ldr: "",
                    hcsr04: ""
                }
            })
        }
    }
})

new Vue({
    render: h => h(App),
    components: {
        FontAwesomeIcon
    },
    store: store
}).$mount('#app')
