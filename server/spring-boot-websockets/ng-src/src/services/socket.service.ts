import { Injectable } from '@angular/core';
import { catchError, EMPTY, Subject, switchAll, tap } from 'rxjs';
import { webSocket, WebSocketSubject } from 'rxjs/webSocket';

const WS_ENDPOINT = "ws://localhost:8080/chat";
@Injectable({
  providedIn: 'root'
})
export class SocketService {

  private socket$?: WebSocketSubject<any>;
  private messageSubject$ = new Subject();
  // public messages$ = this.messageSubject$.pipe(switchAll(), catchError(e => { throw e }));


  constructor() { }

  public connect(): void {

    if (!this.socket$ || this.socket$.closed) {
      this.socket$ = this.getNewWebSocket();
      const messages = this.socket$
        .pipe(
          tap({
            error: error => console.log(error),
          }),
          catchError(_ => EMPTY)
        )
        .subscribe(res => console.log(res))
    }
  }

  private getNewWebSocket() {
    return webSocket(WS_ENDPOINT);
  }
  sendMessage(msg: any) {
    console.log("Sending message")
    this.socket$!.next(msg);
  }
  close() {
    this.socket$!.complete();
  }
}
