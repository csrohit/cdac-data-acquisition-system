package io.e3.gr5.prj;

import io.e3.gr5.prj.service.MessagingService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.CommandLineRunner;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.context.ConfigurableApplicationContext;

@SpringBootApplication
public class PrjApplication implements CommandLineRunner {

    @Autowired
    private MessagingService messagingService;

    @Autowired
    private ConfigurableApplicationContext context;
    public static void main(String[] args) {
        SpringApplication.run(PrjApplication.class, args);
    }

    @Override
    public void run(String... args) throws Exception {
        final String topic = "led";

        messagingService.subscribe(topic);

        messagingService.publish(topic, "Hi\nThis is a sample message published to topic roytuts/topic/event", 0, true);

//        context.close();
    }
}
