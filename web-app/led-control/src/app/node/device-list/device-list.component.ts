import { Component, OnInit } from '@angular/core';
import { ActivatedRoute, Router } from '@angular/router';
import { first, Observable } from 'rxjs';
import { ICommand, IDevice } from 'src/app/model/interfaces';
import { DeviceService } from 'src/app/service/device.service';
import { NodeService } from 'src/app/service/node.service';

@Component({
  selector: 'app-device-list',
  templateUrl: './device-list.component.html',
  styleUrls: ['./device-list.component.scss']
})
export class DeviceListComponent implements OnInit {

  devices$!: Observable<IDevice[]>;
  areCommandsVisible = false;
  selectedDeviceId: IDevice['id'] = -1;

  constructor(
    private nodeService: NodeService,
    private activatedRoute: ActivatedRoute,
    private router: Router,
    private deviceService: DeviceService
  ) { }

  ngOnInit(): void {

    this.activatedRoute.params.subscribe(params => {
      if (params['id']) {
        this.devices$ = this.nodeService
          .findDevices(params['id']);
      }
    })
  }

  /**
   * Navigates to node-list component 
   */
  onGoToNodeClick(): void {
    this.router.navigate(['node']);
  }

  /**
   * Toggle command list
   */
  onViewCommandsClick(id: IDevice['id']): void {
    if(this.selectedDeviceId){
      this.selectedDeviceId = undefined;
    }else{
      this.selectedDeviceId = id;
    }
  }

  onRunCommandClick(id: IDevice['id'], cmdName: ICommand['name']): void {
    this.deviceService.runCommand(id, cmdName).pipe(first()).subscribe();
  }

  /**
   * Event handler for edit device button click
   * @param id id of device
   */
  onDeviceEditClick(id: IDevice['id']): void {
    this.router.navigate(['device', id, 'edit']);
  }

  /**
   * 
   * @param id id of device
   */
  onDeviceDeleteClick(id: IDevice['id']): void {
    this.deviceService.delete(id).pipe(first()).subscribe(() => {
      this.ngOnInit();
    })
  }

}
