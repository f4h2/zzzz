package com.IOT.IOT_system;

import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.boot.autoconfigure.domain.EntityScan;
import org.springframework.boot.autoconfigure.security.servlet.UserDetailsServiceAutoConfiguration;


@SpringBootApplication(exclude= {UserDetailsServiceAutoConfiguration.class})
public class IotSystemApplication {

	public static void main(String[] args) {
		SpringApplication.run(IotSystemApplication.class, args);
	}

}
