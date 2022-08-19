import { NgModule } from '@angular/core';
import { RouterModule, Routes } from '@angular/router';
import { DeviceListComponent } from './device-list/device-list.component';
import { NodeAddComponent } from './node-add/node-add.component';
import { NodeDetailsComponent } from './node-details/node-details.component';
import { NodeListComponent } from './node-list.component';

const routes: Routes = [
  { path: '', component: NodeListComponent },
  { path: 'add', component: NodeAddComponent },
  {
    path: ':id', children: [
      { path: '', component: NodeDetailsComponent },
      { path: 'devices', component: DeviceListComponent },
      { path: 'edit', component: NodeAddComponent}
    ]
  }
];

@NgModule({
  imports: [RouterModule.forChild(routes)],
  exports: [RouterModule]
})
export class NodeRoutingModule { }
