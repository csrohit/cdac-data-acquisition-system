import { Component, OnInit } from '@angular/core';
import { ActivatedRoute } from '@angular/router';
import { Observable } from 'rxjs';
import { IDevice } from '../model/interfaces';
import { NodeService } from '../service/node.service';

@Component({
  selector: 'app-device',
  templateUrl: './device.component.html',
  styleUrls: ['./device.component.scss']
})
export class DeviceComponent implements OnInit {

  devices$!: Observable<IDevice[]>

  constructor(
    private nodeService: NodeService,
    private _route: ActivatedRoute
  ) { 
    this._route.params.subscribe(params => {
      if(params['id']){
        this.devices$ = this.nodeService.findDevices(params['id']);
      }
    })


  }

  ngOnInit(): void {
  }

}
