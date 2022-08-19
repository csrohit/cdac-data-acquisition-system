import { NgModule } from '@angular/core';
import { RouterModule, Routes } from '@angular/router';
import { DeviceAddComponent } from '../device/device-add/device-add.component';
import { DeviceDetailComponent } from '../device/device-detail/device-detail.component';
import { DeviceListComponent } from './device-list/device-list.component';
import { NodeAddComponent } from './node-add/node-add.component';
import { NodeDetailsComponent } from './node-details/node-details.component';
import { NodeListComponent } from './node-list.component';

const routes: Routes = [
  { path: '', component: NodeListComponent },
  { path: 'add', component: NodeAddComponent },
  {
    path: ':nodeId', children: [
      { path: '', component: NodeDetailsComponent },
      { path: 'edit', component: NodeAddComponent },
      {
        path: 'device', children: [
          { path: '', component: DeviceListComponent },
          { path: 'add', component: DeviceAddComponent },
          {
            path: ':deviceId', children: [
              { path: '', component: DeviceDetailComponent },
              { path: 'edit', component: DeviceAddComponent },
            ]
          }
        ]
      }
    ]
  }
];

@NgModule({
  imports: [RouterModule.forChild(routes)],
  exports: [RouterModule]
})
export class NodeRoutingModule { }
