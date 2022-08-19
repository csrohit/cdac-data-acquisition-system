import { NgModule } from '@angular/core';
import { CommonModule } from '@angular/common';
import { MatIconModule } from '@angular/material/icon';

import { NodeRoutingModule } from './node-routing.module';
import { NodeListComponent } from './node-list.component';
import { MatButtonModule } from '@angular/material/button';
import { DeviceListComponent } from './device-list/device-list.component';
import { NodeAddComponent } from './node-add/node-add.component';
import { NodeDetailsComponent } from './node-details/node-details.component';


@NgModule({
  declarations: [
    NodeListComponent,
    DeviceListComponent,
    NodeAddComponent,
    NodeDetailsComponent
  ],
  imports: [
    CommonModule,
    NodeRoutingModule,
    MatIconModule,
    MatButtonModule
  ]
})
export class NodeModule { }
