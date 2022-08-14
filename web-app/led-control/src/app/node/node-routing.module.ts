import { NgModule } from '@angular/core';
import { RouterModule, Routes } from '@angular/router';
import { DeviceListComponent } from './device-list/device-list.component';
import { NodeComponent } from './node.component';

const routes: Routes = [
  { path: '', component: NodeComponent },
  { path: ':id/devices', component: DeviceListComponent }
];

@NgModule({
  imports: [RouterModule.forChild(routes)],
  exports: [RouterModule]
})
export class NodeRoutingModule { }
