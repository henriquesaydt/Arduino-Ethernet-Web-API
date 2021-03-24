<template>
  <div class="d-flex" id="app">
    <a @click="$root.$emit('bv::toggle::collapse', 'sidebarID')" style="cursor: pointer; width: 50px; margin-top: 50px; margin-left: 50px; margin-right: -100px">
      <div class="rounded p-2 text-center" style="width: 50px;">
        <font-awesome-icon style="color: #043f66; width: 50px; height: 50px" icon="bars"/>
      </div>
    </a>
    <Sidebar/>
    <Home class="text-center pl-0 pr-0 pl-xl-3 pr-xl-3"/>
  </div>
</template>

<script>
import Home from './components/home';
import Sidebar from './components/Sidebar'
import './custom.scss';

export default {
  name: 'App',
  components: {
    Home,
    Sidebar
  }
}
</script>

<style>
  body {
    background-color:#c8d0da;
  }
</style>