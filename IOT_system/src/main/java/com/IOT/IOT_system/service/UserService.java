package com.IOT.IOT_system.service;

import com.IOT.IOT_system.DTO.LoginDTO;
import com.IOT.IOT_system.DTO.Login_response;
import com.IOT.IOT_system.DTO.UserDTO;
import com.IOT.IOT_system.model.User;

public interface UserService {
    public User addUser(UserDTO userDTO);
    public Login_response loginUser(LoginDTO loginDTO);

}