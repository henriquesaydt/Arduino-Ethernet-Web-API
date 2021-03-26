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
import { FontAwesomeIcon } from '@fortawesome/vue-fontawesome'

library.add(faLightbulb)
library.add(faThermometerHalf)
library.add(faMicrochip)
library.add(faUpload)
library.add(faBars)
library.add(faCircle)

Vue.use(Vuex)

Vue.component('font-awesome-icon', FontAwesomeIcon)

Vue.config.productionTip = false

const store = new Vuex.Store({
  state: {
    config: {
      ip: "",
      port: "",
      token: ""
    },
    status: {text: "Desconectado", color: "red", error: ""}
  },
  getters: {
    status: state => {
      return state.status;
    }
  },
  mutations: {
    connect(state, {ip, port, token}) {
      state.status = {text: "Conectando", color: "#d69d00", error: ""};
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
      })
      .then( response => {
        if (response.data.status == "success") {
          state.status = {text: "Conectado", color: "green"};
        }
        else {
          state.status = {text: "Desconectado", color: "red", error: response.data.message};
        }
      })
    },
    desconnect(state) {
      state.config = {
        ip: "",
        port: "",
        token: ""
      }
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
