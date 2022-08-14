import { Component } from '@angular/core';
import { Router } from '@angular/router';

@Component({
  selector: 'app-root',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.scss']
})
export class AppComponent {
  title = 'led-control';


  constructor(
    private router: Router
  ){}




  onViewNodesClick(): void{
    this.router.navigate(['node']);
  }



}
