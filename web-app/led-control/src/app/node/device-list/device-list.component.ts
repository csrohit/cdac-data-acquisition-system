import { Component, OnInit } from '@angular/core';
import { ActivatedRoute, Router } from '@angular/router';
import { first, Observable } from 'rxjs';
import { ICommand, IDevice, INode } from 'src/app/model/interfaces';
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
  selectedDeviceId: IDevice['id'] = undefined;
  selectedNodeId: INode['id'] = undefined

  constructor(
    private nodeService: NodeService,
    private activatedRoute: ActivatedRoute,
    private router: Router,
    private deviceService: DeviceService
  ) { }

  ngOnInit(): void {

    this.activatedRoute.params.subscribe(params => {
      this.selectedNodeId = params['nodeId'];
      if (params['nodeId']) {
        this.devices$ = this.nodeService
          .findDevices(params['nodeId']);
      }else{

        this.devices$ = this.deviceService
          .findAll();
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
    if (this.selectedNodeId) {
      this.router.navigate(['node', this.selectedNodeId, 'device', id, 'edit']);
    } else {
      this.router.navigate(['device', id, 'edit'])
    }
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
