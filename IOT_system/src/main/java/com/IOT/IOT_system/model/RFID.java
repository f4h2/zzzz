package com.IOT.IOT_system.model;

import jakarta.persistence.*;

import java.util.Date;


@Entity
@Table(name = "rfid")
public class RFID {
    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    @Column(name = "id")
    private Long id;
    private String IDx;
    private Date date;

    public RFID() {

    }

    public String getTopic() {
        return topic;
    }

    public RFID(Long id, String IDx, Date date, String topic) {
        this.id = id;
        this.IDx = IDx;
        this.date = date;
        this.topic = topic;
    }

    public Date getDate() {
        return date;
    }

    public void setDate(Date date) {
        this.date = date;
    }

    public void setTopic(String topic) {
        this.topic = topic;
    }

    private String topic;
    public Long getId() {
        return id;
    }

    public void setId(Long id) {
        this.id = id;
    }

    public String getIDx(String myMessage) {
        return IDx;
    }

    public void setIDx(String IDx) {
        this.IDx = IDx;
    }
}
