package com.IOT.IOT_system.controller;

import com.IOT.IOT_system.DTO.Login_response;
import com.IOT.IOT_system.DTO.UserDTO;
import com.IOT.IOT_system.DTO.LoginDTO;
import com.IOT.IOT_system.model.User;
import com.IOT.IOT_system.repository.UserDTO_Repository;
import com.IOT.IOT_system.service.impl.UserImpl;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.ResponseEntity;
import org.springframework.security.access.prepost.PreAuthorize;
import org.springframework.web.bind.annotation.*;

import java.util.HashMap;
import java.util.List;
import java.util.Map;


@RestController
@CrossOrigin
@RequestMapping("/user")
public class UserController {

    @Autowired
    private UserImpl userService;
    @Autowired
    private UserDTO_Repository userDTOrepo;


    @PostMapping(path = "/saveUser")
    @PreAuthorize("hasAuthority('ROLE_ADMIN') ")
    public User addUser_to_DB(@RequestBody UserDTO userDTO)
    {
        User userx = userService.addUser(userDTO);
        return userx;
    }
    @PostMapping(path = "/saveUserDTO")
    public UserDTO addUserDTO_to_DB(@RequestBody UserDTO userDTO)
    {
        System.out.println("da call api");
        return userDTOrepo.save(userDTO);
    }

    @PostMapping(path = "/login")
    @PreAuthorize("hasAuthority('ROLE_ADMIN') and hasAuthority('ROLE_USER')")
    public Login_response loginUser(@RequestBody LoginDTO loginDTO)
    {
        Login_response login_response =  userService.loginUser(loginDTO);
        return login_response;
    }

    @PreAuthorize("hasAuthority('ROLE_ADMIN')")
    @GetMapping(path = "/listDTO")
    public List<UserDTO> getAllUserDTO() {
        return userDTOrepo.findAll();
    }

    @DeleteMapping (path = "/deletex/{id}")
    @PreAuthorize("hasAuthority('ROLE_ADMIN')")
    public ResponseEntity<Map<String, Boolean>>deleteUser(@PathVariable Long id){
        userService.deleteUser(id);
        Map<String, Boolean> response = new HashMap<>();
        response.put("deleted", Boolean.TRUE);
        return ResponseEntity.ok(response);
    }

}

