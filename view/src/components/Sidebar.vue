<template>
    <div>
        <b-sidebar ref="sidebar" id="sidebarID" title="Configurações" backdrop backdrop-variant="" shadow>
            <div class="px-3 py-2">
                <b-row>
                    <b-col cols="3">
                        <b-form-group label-cols="3" style="font-weight: bold" label="Status:" label-for="status"/>
                    </b-col>
                    <b-col cols="1">
                        <font-awesome-icon class="text-danger" style="margin-top: 12px" icon="circle"/>
                    </b-col>
                    <b-col cols="7" class="text-left">
                        <h6 style="margin-top: 9px"><b>Desconectado</b></h6>
                    </b-col>
                </b-row>
                <b-row>
                    <b-col cols="8"> 
                        <b-form-group style="font-weight: bold" label="Endereço:" label-for="ip">
                            <b-input id="ip"/>
                        </b-form-group>
                    </b-col>
                    <b-col cols="4"> 
                        <b-form-group style="font-weight: bold" label="Porta:" label-for="porta">
                            <b-input id="porta"/>
                        </b-form-group>
                    </b-col>
                </b-row>
                <b-row>
                    <b-col cols="12">
                        <b-form-group style="font-weight: bold" label="Token:" label-for="token">
                            <b-input id="token" type="password"/>
                        </b-form-group>
                    </b-col>
                </b-row>
                <b-row class="mt-3">
                    <b-col class="text-center" cols="6">
                        <b-button variant="danger">Desconectar</b-button>
                    </b-col>
                    <b-col class="text-center" cols="6">
                        <b-button variant="success">Conectar</b-button>
                    </b-col>
                </b-row>
            </div>
        </b-sidebar>
    </div>
</template>