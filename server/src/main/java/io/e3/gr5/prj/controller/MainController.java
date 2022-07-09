package io.e3.gr5.prj.controller;

@Controller()
@RequestMapping("")
public class MainController {


    @GetMapping("/")
    public String index(){
        return "index.html";
    }

    @MessageMapping("/chat")
    @SendTo("/topic/messages")
    public OutputMessage send(Message message) throws Exception {
        String time = new SimpleDateFormat("HH:mm").format(new Date());
        return new OutputMessage(message.getFrom(), message.getText(), time);
    }


}