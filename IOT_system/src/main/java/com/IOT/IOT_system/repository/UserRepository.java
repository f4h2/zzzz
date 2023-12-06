package com.IOT.IOT_system.repository;

import com.IOT.IOT_system.model.User;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.Query;

import java.util.Optional;

public interface UserRepository extends JpaRepository<User,Integer> {

    @Query(value = "SELECT * FROM user WHERE ten_tk=?",nativeQuery = true)
    Optional<User> findByUserName(String ten_tk);
    @Query(value = "SELECT * FROM user WHERE tk_user=? AND mk_user=?",nativeQuery = true)
    Optional<User> findOneByTkAndMk(String tk_user, String mk_user);
    Optional<User> findById(Long id);

}
