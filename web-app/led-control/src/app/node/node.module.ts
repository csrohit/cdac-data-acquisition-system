import { NgModule } from '@angular/core';
import { CommonModule } from '@angular/common';
import { MatIconModule } from '@angular/material/icon';

import { NodeRoutingModule } from './node-routing.module';
import { NodeComponent } from './node.component';
import { MatButtonModule } from '@angular/material/button';
import { DeviceListComponent } from './device-list/device-list.component';


@NgModule({
  declarations: [
    NodeComponent,
    DeviceListComponent
  ],
  imports: [
    CommonModule,
    NodeRoutingModule,
    MatIconModule,
    MatButtonModule
  ]
})
export class NodeModule { }
