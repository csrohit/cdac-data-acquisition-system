import { NgModule } from '@angular/core';
import { RouterModule, Routes } from '@angular/router';
import { DeviceListComponent } from '../node/device-list/device-list.component';
import { DeviceAddComponent } from './device-add/device-add.component';
import { DeviceDetailComponent } from './device-detail/device-detail.component';
import { DeviceComponent } from './device.component';

const routes: Routes = [
  { path: '', component: DeviceListComponent },
  { path: 'add', component: DeviceAddComponent },
  {
    path: ':deviceId', children: [
      { path: '', component: DeviceDetailComponent },
      { path: 'edit', component: DeviceAddComponent },
    ]
  }
];

@NgModule({
  imports: [RouterModule.forChild(routes)],
  exports: [RouterModule]
})
export class DeviceRoutingModule { }
