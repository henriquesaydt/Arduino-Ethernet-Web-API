import '@babel/polyfill'
import 'mutationobserver-shim'
import Vue from 'vue'
import './plugins/bootstrap-vue'
import App from './App.vue'
import Vuex from 'vuex'

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
    }
  },
  mutations: {
    setConfig(state, {ip, port, token}) {
      state.config = {
        ip: ip,
        port: port,
        token: token
      }
      
    },
    desconect(state) {
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
