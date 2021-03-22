<template>
  <div id="app">
    <Home/>
  </div>
</template>

<script>
import Home from './components/home';
import './custom.scss';

export default {
  name: 'App',
  components: {
    Home
  }
}
</script>

<style>
  body {
    background-color:#c8d0da;
  }
</style>