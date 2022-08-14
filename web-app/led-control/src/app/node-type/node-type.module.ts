import { NgModule } from '@angular/core';
import { CommonModule } from '@angular/common';

import { NodeTypeRoutingModule } from './node-type-routing.module';
import { NodeTypeComponent } from './node-type.component';


@NgModule({
  declarations: [
    NodeTypeComponent
  ],
  imports: [
    CommonModule,
    NodeTypeRoutingModule
  ]
})
export class NodeTypeModule { }
