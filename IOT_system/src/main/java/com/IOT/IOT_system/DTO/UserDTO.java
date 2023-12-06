package com.IOT.IOT_system.DTO;

import jakarta.persistence.*;

@Entity
@Table(name = "USERDTO")
public class UserDTO {
    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    @Column(name = "id")
    private Long user_id;
    private String user_ten;
    private String user_tk;
    private String user_mk;
    private String role;

    public UserDTO() {
    }

    public String getRole() {
        return role;
    }

    public void setRole(String role) {
        this.role = role;
    }

    public Long getUser_id() {
        return user_id;
    }

    public void setUser_id(Long user_id) {
        this.user_id = user_id;
    }

    public String getUser_ten() {
        return user_ten;
    }

    public void setUser_ten(String user_ten) {
        this.user_ten = user_ten;
    }

    public String getUser_tk() {
        return user_tk;
    }

    public void setUser_tk(String user_tk) {
        this.user_tk = user_tk;
    }

    public String getUser_mk() {
        return user_mk;
    }

    public void setUser_mk(String user_mk) {
        this.user_mk = user_mk;
    }

    public UserDTO(Long user_id, String user_ten, String user_tk, String user_mk) {
        this.user_id = user_id;
        this.user_ten = user_ten;
        this.user_tk = user_tk;
        this.user_mk = user_mk;
    }


}