<template>
    <div>
        <b-sidebar ref="sidebar" id="sidebarID" title="Configurações" backdrop backdrop-variant="" shadow>
            <div class="px-3 py-2">
                <b-form @submit="connect($event)" ref="formConfig">
                    <b-row>
                        <b-col cols="3">
                            <b-form-group label-cols="3" style="font-weight: bold" label="Status:" label-for="status"/>
                        </b-col>
                        <b-col cols="1">
                            <font-awesome-icon :style="{'margin-top':'12px', 'color':$store.getters.status.color}" icon="circle"/>
                        </b-col>
                        <b-col cols="7" class="text-left">
                            <h6 style="margin-top: 9px"><b>{{$store.getters.status.text}}</b></h6>
                        </b-col>
                    </b-row>
                    <b-row>
                        <b-col cols="8"> 
                            <b-form-group style="font-weight: bold" label="Endereço:" label-for="ip">
                                <b-input id="ip" name="ip" required/>
                            </b-form-group>
                        </b-col>
                        <b-col cols="4"> 
                            <b-form-group style="font-weight: bold" label="Porta:" label-for="porta">
                                <b-input id="port"  name="port" required/>
                            </b-form-group>
                        </b-col>
                    </b-row>
                    <b-row>
                        <b-col cols="12">
                            <b-form-group style="font-weight: bold" label="Token:" label-for="token">
                                <b-input id="token" name="token" type="password" required/>
                            </b-form-group>
                        </b-col>
                    </b-row>
                    <b-row class="mt-3">
                        <b-col class="text-center" cols="6">
                            <b-button @click="desconnect" variant="danger" :disabled="$store.getters.status.id==0">Desconectar</b-button>
                        </b-col>
                        <b-col class="text-center" cols="6">
                            <b-button type="submit" variant="success" :disabled="$store.getters.status.id==2">Conectar</b-button>
                        </b-col>
                    </b-row>
                    <b-row class="mt-4">
                        <b-col class="text-center">
                            <b-spinner v-if="$store.getters.status.id == 1"/>
                            <h5 v-else-if="$store.getters.status.id == 0 && $store.getters.status.error"><span class="text-danger">Erro: </span>{{$store.getters.status.error}}</h5>
                        </b-col>
                    </b-row>
                </b-form>
            </div>
        </b-sidebar>
    </div>
</template>

<script>
export default {
    data() {
        return {
        }
    },

    methods: {
        connect(event) {
            event.preventDefault();
            const form = new FormData(this.$refs.formConfig);
            this.$store.commit('connect', {ip: form.get('ip'), port: form.get('port'), token: form.get('token')});
        },
        desconnect() {
            this.$store.commit('desconnect');
        }
    },
}
</script>
