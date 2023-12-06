package com.IOT.IOT_system.service.impl;

import com.IOT.IOT_system.DTO.LoginDTO;
import com.IOT.IOT_system.DTO.Login_response;
import com.IOT.IOT_system.DTO.UserDTO;
import com.IOT.IOT_system.model.User;
import com.IOT.IOT_system.repository.UserRepository;
import com.IOT.IOT_system.service.UserService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.security.crypto.password.PasswordEncoder;
import org.springframework.stereotype.Service;

import java.util.List;
import java.util.Optional;

@Service
public class UserImpl implements UserService {
    @Autowired
    private UserRepository userRepo;

    private PasswordEncoder passwordEncoder;
    @Override
    public User addUser(UserDTO userDTO) {
        User user = new User(
                (long) userDTO.getUser_id(),
                userDTO.getUser_ten(),
                userDTO.getUser_tk(),
                this.passwordEncoder.encode(userDTO.getUser_mk()),
                userDTO.getRole()
        );
        return userRepo.save(user);
    }
    UserDTO userDTO;
    @Override
    public Login_response  loginUser(LoginDTO loginDTO) {
        User user = null;
        String message="";
        Optional<User> userOptional = userRepo.findByUserName(loginDTO.getUser_tk());
        if (userOptional.isPresent()) {
            user = userOptional.get();
            String password = loginDTO.getUser_mk();
            String encodedPassword = user.getMk_user();
            Boolean isPwdRight = passwordEncoder.matches(password, encodedPassword);
            if (isPwdRight) {

                    message = "Login Ok";

            } else {
                message = "password Not Match";
            }
        }
        else {
             message = "Email not exits";
        }

        assert user != null;
        Login_response login_response = new Login_response(message, user.getTk_user(),user.getRoles() );
        return login_response;
    }

    public String deleteUser( Long id){
        Optional<User> userOptional = userRepo.findById(id);
        if(userOptional.isPresent()) {
            User user = userOptional.get();
            userRepo.delete(user);
            return "delete ok";
        }else{
            return "delete -ok";
        }
    }
    public List<User> listAll() {
        return userRepo.findAll();
    }
}
