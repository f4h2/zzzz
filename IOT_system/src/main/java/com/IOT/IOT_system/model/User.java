package com.IOT.IOT_system.model;

import jakarta.persistence.*;


@Entity
@Table(name = "users")
public class User {

    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    @Column(name = "id")
    private Long id;

    private String ten_tk;
    private String tk_user;

    public User() {

    }

    public String getTen_tk() {
        return ten_tk;
    }

    public User(Long id, String ten_tk, String tk_user, String mk_user, String roles) {
        this.id = id;
        this.ten_tk = ten_tk;
        this.tk_user = tk_user;
        this.mk_user = mk_user;
        this.roles = roles;
    }

    public void setTen_tk(String ten_tk) {
        this.ten_tk = ten_tk;
    }

    private String mk_user;
    private String roles;

    public Long getId() {
        return id;
    }

    public void setId(Long id) {
        this.id = id;
    }

    public String getTk_user() {
        return tk_user;
    }

    public void setTk_user(String tk_user) {
        this.tk_user = tk_user;
    }

    public String getMk_user() {
        return mk_user;
    }

    public void setMk_user(String mk_user) {
        this.mk_user = mk_user;
    }

    public String getRoles() {
        return roles;
    }

    public void setRoles(String roles) {
        this.roles = roles;
    }





}