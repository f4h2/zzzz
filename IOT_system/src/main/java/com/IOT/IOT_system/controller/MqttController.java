package com.IOT.IOT_system.controller;

import com.IOT.IOT_system.model.RFID;
import com.IOT.IOT_system.service.MqttGateway;
import com.fasterxml.jackson.databind.ObjectMapper;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RestController;

@RestController
public class MqttController {

    private final MqttGateway mqtGateway;
    private final ObjectMapper objectMapper;

    public MqttController(MqttGateway mqtGateway, ObjectMapper objectMapper) {
        this.mqtGateway = mqtGateway;
        this.objectMapper = objectMapper;
    }

    @PostMapping("/sendMessage")
    public ResponseEntity<?> publish(@RequestBody RFID mqttMessage) {
        try {
            mqtGateway.senToMqtt(objectMapper.writeValueAsString(mqttMessage), mqttMessage.getTopic());
            return ResponseEntity.ok("Success");
        } catch (Exception ex) {
            ex.printStackTrace();
            return ResponseEntity.ok("fail");
        }
    }
}