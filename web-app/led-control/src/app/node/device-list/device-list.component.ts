import { Component, OnInit } from '@angular/core';
import { ActivatedRoute } from '@angular/router';
import { Observable } from 'rxjs';
import { IDevice } from 'src/app/model/interfaces';
import { NodeService } from 'src/app/service/node.service';

@Component({
  selector: 'app-device-list',
  templateUrl: './device-list.component.html',
  styleUrls: ['./device-list.component.scss']
})
export class DeviceListComponent implements OnInit {

  devices$!: Observable<IDevice[]>;

  constructor(
    private nodeService: NodeService,
    private activatedRoute: ActivatedRoute
  ) { }

  ngOnInit(): void {

    this.activatedRoute.params.subscribe(params => {
      if (params['id']) {
        this.devices$ = this.nodeService
          .findDevices(params['id']);
      }
    })


  }

}
