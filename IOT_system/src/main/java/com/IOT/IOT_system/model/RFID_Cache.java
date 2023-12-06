//package com.IOT.IOT_system.model;
//
//import jakarta.persistence.Id;
//import java.io.Serializable;
//import lombok.AllArgsConstructor;
//import lombok.Builder;
//import lombok.Data;
//import lombok.NoArgsConstructor;
//import org.springframework.data.redis.core.RedisHash;
//import org.springframework.data.redis.core.index.Indexed;
//
//@Data
//@Builder
//@NoArgsConstructor
//@AllArgsConstructor
//@RedisHash("RFID")
//public class RFID_Cache implements Serializable {
//    @Indexed
//    @Id String id;
//
//    @Indexed private String IDx;
//
//
//    public RFID_Cache(RFID rfid) {
//        this.IDx = rfid.getIDx();
//    }
//}
