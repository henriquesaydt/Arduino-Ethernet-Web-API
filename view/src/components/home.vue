<template>
    <b-container class="mt-2 mt-md-3 mt-xl-5 mb-5">
        <div class="pl-4 pr-4 pb-3 pt-3 p-xl-5 bg-light" style="border-radius: 5px">
            <b-row>
                <b-col md="6">

                    <div class="text-light mb-2 mb-xl-4" style="background-color: #043f66; padding: 1px; border-radius: 5px; font-size: 25px;font-weight: bold">
                        <b-row>
                            <b-col cols="3">
                            </b-col>
                            <b-col cols="6" class="text-center">
                                Entradas <font-awesome-icon icon="microchip"/>
                            </b-col>
                            <b-col cols="3" class="text-center">
                                <a @click="sendData" style="cursor: pointer">
                                    <font-awesome-icon icon="upload"/>
                                </a>
                            </b-col>    
                        </b-row>
                    </div>

                    <b-form ref="formEntradas">
                        <b-row>
                            <b-col cols="6" md="3">
                                <b-form-group style="color: red; font-weight: bold" label="LED 1:" label-for="led_1">
                                    <b-form-checkbox id="led_1" name="led_1" size="lg" switch/>
                                </b-form-group>
                            </b-col>
                            <b-col style="color: green; font-weight: bold" cols="6" md="3">
                                <b-form-group label="LED 2:" label-for="led_2">
                                    <b-form-checkbox id="led_2" name="led_2" size="lg" switch/>
                                </b-form-group>
                            </b-col>
                            <b-col style="color: blue; font-weight: bold" cols="6" md="3">
                                <b-form-group label="LED 3:" label-for="led_3">
                                    <b-form-checkbox id="led_3" name="led_3" size="lg" switch/>
                                </b-form-group>
                            </b-col>
                            <b-col cols="6" md="3" style="font-weight: bold">
                                <b-form-group :style="'color:' + led_rgb" label="RGB:" label-for="led_rgb">
                                    <b-form-input id="led_rgb" name="led_rgb" v-model="led_rgb" type="color"/>
                                </b-form-group>
                            </b-col>
                        </b-row>
                        <b-row>
                            <b-col cols="6" md="6">
                                <label style="font-weight: bold" for="servo">Servo motor: {{value}}°</label>
                                <b-form-input id="servo" name="servo" v-model="value" type="range" min="0" max="180" step="10"></b-form-input>
                            </b-col>
                            <b-col cols="6" md="6">
                                <b-form-group style="font-weight: bold" label="Display Nokia:" label-for="display">
                                    <b-input id="display" name="display"/>
                                </b-form-group>
                            </b-col>
                        </b-row>
                    </b-form>

                </b-col>

                <b-col md="6">

                    <div class="text-light mb-2 mb-xl-4" style="background-color: #043f66; padding: 1px; border-radius: 5px; font-size: 25px;font-weight: bold">
                        <b-row>
                            <b-col cols="3">
                            </b-col>
                            <b-col cols="6" class="text-center">
                                Sensores <font-awesome-icon icon="thermometer-half"/>
                            </b-col>
                            <b-col cols="3" class="text-center">
                                <a @click="getData" style="cursor: pointer">
                                    <font-awesome-icon icon="sync" :spin="$store.getters.gettingData"/>
                                </a>
                            </b-col>    
                        </b-row>
                    </div>
                    <b-row>
                        <b-col cols="6" md="6" lg="4">
                            <b>LM35:</b>
                            <b-input-group class="mt-2" append="°C">
                                <b-form-input disabled type="number" :value="$store.getters.sensorsData.lm35"></b-form-input>
                            </b-input-group>
                        </b-col>
                        <b-col cols="6" md="6" lg="4">
                            <b>KY-038:</b>
                            <b-input-group class="mt-2" append="V">
                                <b-form-input disabled type="number" :value="$store.getters.sensorsData.ky038"></b-form-input>
                            </b-input-group>
                        </b-col>
                        <b-col cols="6" md="6" lg="4" class="mt-2 mt-sm-0">
                            <b>HC-SR04:</b>
                            <b-input-group class="mt-2" append="cm">
                                <b-form-input disabled type="number" :value="$store.getters.sensorsData.hcsr04"></b-form-input>
                            </b-input-group>
                        </b-col>
                    </b-row>

                </b-col>
                
            </b-row>
            
            <div class="text-center mt-3 mt-xl-4 w-75 w-xl-100" style="max-width: 800px; display: block; margin: auto;">
                <b-embed
                    type="iframe"
                    aspect="16by9"
                    src="https://www.youtube.com/embed/IRB7E0aLxWE"
                    allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture"
                    allowfullscreen
                ></b-embed>
            </div>
        </div>
    </b-container>
</template>

<script>

export default {
    data() {
        return {
            config: {
                endereco: "",
                porta: "",
                token: ""
            },
            value: 0,
            led_rgb: null,
            autoLoad: false,
        }
    },

    watch: {
        IDStatus(val) {
            if (val == 2) {
                this.autoLoad = true;
                this.$store.commit("getData", {reference: this});
                setInterval(() => {
                    if (!this.autoLoad) {
                        return;
                    }
                    this.$store.commit("getData", {reference: this});
                }, 5000)
            }
            else {
                this.autoLoad = false;
            }
        }
    },

    computed: {
        hexToRgb() {
            var hex = this.led_rgb;
            if (hex) {
                var shorthandRegex = /^#?([a-f\d])([a-f\d])([a-f\d])$/i;
                hex = hex.replace(shorthandRegex, function(m, r, g, b) {
                    return r + r + g + g + b + b;
                });

                var result = /^#?([a-f\d]{2})([a-f\d]{2})([a-f\d]{2})$/i.exec(hex);
                return result ? {
                    r: parseInt(result[1], 16),
                    g: parseInt(result[2], 16),
                    b: parseInt(result[3], 16)
                } : null;
            }
            else {
                return null;
            }
        },
        IDStatus() {
            return this.$store.getters.status.id;
        }
    },

    methods: {
        sendData() {
            if (this.$store.getters.status.id != 2) {
                this.$bvToast.toast('Não é possível enviar os dados, é necessário primeiro se conectar ao Arduino no menu de configurações', {
                    title: "Não há conexão com o Arduino",
                    variant: "warning",
                    solid: true
                })
                return;
            }
            var formulario = new FormData(this.$refs.formEntradas);
            var formJson = {
                token: this.$store.getters.config.token,
                data: {
                    led_1: formulario.get("led_1"),
                    led_2: formulario.get("led_2"),
                    led_3: formulario.get("led_3"),
                    led_rgb: this.hexToRgb,
                    servo: formulario.get("servo"),
                    display: formulario.get("display")
                }
            }
            this.$store.commit("sendEntries", {data: formJson, reference: this});
        },
        getData() {
            if (this.$store.getters.status.id != 2) {
                this.$bvToast.toast('Não é possível receber os dados, é necessário primeiro se conectar ao Arduino no menu de configurações', {
                    title: "Não há conexão com o Arduino",
                    variant: "warning",
                    solid: true
                })
                return;
            }
            this.$store.commit("getData", {reference: this});
        }
    },
}
</script>