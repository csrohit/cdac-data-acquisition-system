import { NgModule } from '@angular/core';
import { CommonModule } from '@angular/common';

import { DeviceTypeRoutingModule } from './device-type-routing.module';
import { DeviceTypeComponent } from './device-type.component';


@NgModule({
  declarations: [
    DeviceTypeComponent
  ],
  imports: [
    CommonModule,
    DeviceTypeRoutingModule
  ]
})
export class DeviceTypeModule { }
