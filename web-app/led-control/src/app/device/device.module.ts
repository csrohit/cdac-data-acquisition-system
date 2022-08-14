import { NgModule } from '@angular/core';
import { CommonModule } from '@angular/common';

import { DeviceRoutingModule } from './device-routing.module';
import { DeviceComponent } from './device.component';
import { MatButtonModule } from '@angular/material/button';
import { MatIconModule } from '@angular/material/icon';


@NgModule({
  declarations: [
    DeviceComponent
  ],
  imports: [
    CommonModule,
    DeviceRoutingModule,
    MatButtonModule,
    MatIconModule
  ]
})
export class DeviceModule { }
