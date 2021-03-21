<template>
    <b-container class="mt-5 mb-5">
        <div class="p-5 bg-light" style="border-radius: 5px">
            <b-row>
                <b-col md="6">

                    <div class="text-light text-center mb-4" style="background-color: #043f66; padding: 1px; border-radius: 5px; font-size: 25px;font-weight: bold">
                        Entradas <font-awesome-icon icon="microchip"/>
                    </div>

                    <b-row>
                        <b-col cols="6" md="3">
                            <b-form-group style="color: red; font-weight: bold" label="LED 1:" label-for="led_1">
                                <b-form-checkbox id="led_1" size="lg" switch/>
                            </b-form-group>
                        </b-col>
                        <b-col style="color: green; font-weight: bold" cols="6" md="3">
                            <b-form-group label="LED 2:" label-for="led_2">
                                <b-form-checkbox id="led_2" size="lg" switch/>
                            </b-form-group>
                        </b-col>
                        <b-col style="color: blue; font-weight: bold" cols="6" md="3">
                            <b-form-group label="LED 3:" label-for="led_3">
                                <b-form-checkbox id="led_3" size="lg" switch/>
                            </b-form-group>
                        </b-col>
                        <b-col cols="6" md="3" style="font-weight: bold">
                            <b-form-group :style="'color:' + led_rgb" label="RGB:" label-for="led_rgb">
                                <b-form-input id="led_rgb" v-model="led_rgb" type="color"/>
                            </b-form-group>
                        </b-col>
                    </b-row>
                    <b-row>
                        <b-col cols="6" md="6">
                            <label style="font-weight: bold" for="range-2">Servo motor</label>
                            <b-form-input id="range-2" v-model="value" type="range" min="0" max="180" step="10"></b-form-input>
                            <div class="mt-2">Value: {{ value }}°</div>
                        </b-col>
                        <b-col cols="6" md="6">
                            <b-form-group style="font-weight: bold" label="Display 16x2:" label-for="display16x2">
                                <b-input id="display16x2"/>
                            </b-form-group>
                        </b-col>
                    </b-row>

                </b-col>

                <b-col md="6">

                    <div class="text-light text-center mb-4" style="background-color: #043f66; padding: 1px; border-radius: 5px; font-size: 25px;font-weight: bold">
                        Sensores <font-awesome-icon icon="thermometer-half"/>
                    </div>

                    <b-row>
                        
                    </b-row>

                </b-col>
                
            </b-row>
            
            <div class="text-center mt-3" style="max-width: 700px; display: block; margin: auto;">
                <b-embed
                    type="iframe"
                    aspect="16by9"
                    src="https://www.youtube.com/embed/0nCOQ0fs-So"
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
            value: 0,
            led_rgb: null
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
        }
    }
}
</script>

<style>
    body {
        background-color:#DBE2E4;
    }
</style>