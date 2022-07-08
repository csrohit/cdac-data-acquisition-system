package io.e3.gr5.prj.controller;

import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

@RestController
@RequestMapping("api")
public class StaticController {


    @GetMapping
    public static ResponseEntity<String> hello(){

        return ResponseEntity.ok().body("Hello");
    }

    @PostMapping
    public  static  ResponseEntity<String> world(){
        return ResponseEntity.ok().body("World");
    }
}
