package com.IOT.IOT_system.DTO;

public class Login_response {
    private String message;
    private String tk;
    private String Role;

    public String getTk() {
        return tk;
    }

    public void setTk(String tk) {
        this.tk = tk;
    }

    public Login_response(String message, String tk, String Role) {
        this.message = message;
        this.tk = tk;
        this.Role = Role;
    }

    public String getRole() {
        return Role;
    }

    public void setRole(String role) {
        Role = role;
    }

    public String getMessage() {
        return message;
    }

    public void setMessage(String message) {
        this.message = message;
    }


}
