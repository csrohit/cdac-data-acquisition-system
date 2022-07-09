import { Component } from '@angular/core';
import { SocketService } from 'src/services/socket.service';

@Component({
  selector: 'app-root',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.scss']
})
export class AppComponent {
  title = 'sockets';

  constructor(
    private socketService: SocketService
  ){

    this.socketService.connect();
  }

  send(): void{

    this.socketService.sendMessage({"hello": "world"});
  }
}
