import { Component, OnInit } from '@angular/core';
import { FormControl, FormGroup, Validators } from '@angular/forms';
import { ActivatedRoute, Router } from '@angular/router';
import { first, Observable, of, switchMap } from 'rxjs';
import { IDevice, IDeviceType, INode } from 'src/app/model/interfaces';
import { DeviceTypeService } from 'src/app/service/device-type.service';
import { DeviceService } from 'src/app/service/device.service';
import { NodeService } from 'src/app/service/node.service';

@Component({
  selector: 'app-device-add',
  templateUrl: './device-add.component.html',
  styleUrls: ['./device-add.component.scss']
})
export class DeviceAddComponent implements OnInit {
  device: IDevice = {};
  deviceForm: FormGroup;
  selectedDeviceId: IDevice['id'];
  selectedNodeId: INode['id'] = undefined;
  deviceTypes$: Observable<IDeviceType[]>;
  nodes$: Observable<INode[]>;


  constructor(
    private router: Router,
    private deviceService: DeviceService,
    private deviceTypeService: DeviceTypeService,
    private nodeService: NodeService,
    private route: ActivatedRoute
  ) {
    this.deviceForm = new FormGroup({
      label: new FormControl(this.device.label, Validators.required),
      description: new FormControl(this.device.description, Validators.required),
      hwId: new FormControl(this.device.hwId, Validators.required),
      type: new FormControl(this.device.type?.id, Validators.required),
      node: new FormControl(this.device.node?.id, Validators.required),
    });

    this.deviceTypes$ = this.deviceTypeService.findAll();
    this.nodes$ = this.nodeService.findAll();

  }

  ngOnInit(): void {
    this.route.params.pipe(
      switchMap(params => {
        this.selectedNodeId = params['nodeId'];
        if (params['deviceId']) {
          this.selectedDeviceId = params['deviceId'];
          return this.deviceService.findById(this.selectedDeviceId);
        }
        return of(null);
      })
    ).subscribe(res => {
      if (res) {
        this.device = res;
        this.deviceForm.patchValue({
          ...res,
          type: res.type?.id,
          node: res.node?.id
        })
      }
    });
  }

  /**
   * Event handler for submit button click
   */
  onSubmitClick(): void {
    this.device = {
      label: this.deviceForm.value.label,
      description: this.deviceForm.value.description,
      hwId: this.deviceForm.value.hwId,
      node: {
        id: this.deviceForm.value.node
      },
      type: {
        id: this.deviceForm.value.type
      }
    }

    this.deviceService.save(this.device).pipe(first()).subscribe(() => {
      if (this.selectedNodeId) {
        this.router.navigate(['node', this.selectedNodeId, 'device']);
      } else {
        this.router.navigate(['device'])
      }

    });
  }

  /**
   * Event handler for cancel button click
   */
  onCancelClick(): void {
    if (this.selectedNodeId) {
      this.router.navigate(['node', this.selectedNodeId, 'device']);
    } else {
      this.router.navigate(['device'])
    }
  }

}
