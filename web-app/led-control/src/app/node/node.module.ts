import { NgModule } from '@angular/core';
import { CommonModule } from '@angular/common';
import { MatIconModule } from '@angular/material/icon';
import { MatButtonModule } from '@angular/material/button';
 import { MatFormFieldModule } from '@angular/material/form-field';
 import { MatInputModule } from '@angular/material/input';
 import { MatSelectModule } from '@angular/material/select';
 import { MatRadioModule } from '@angular/material/radio';
 import { MatTooltipModule } from '@angular/material/tooltip';

import { NodeRoutingModule } from './node-routing.module';
import { NodeListComponent } from './node-list.component';
import { DeviceListComponent } from './device-list/device-list.component';
import { NodeAddComponent } from './node-add/node-add.component';
import { NodeDetailsComponent } from './node-details/node-details.component';
import { ReactiveFormsModule } from '@angular/forms';


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
    ReactiveFormsModule,
    MatIconModule,
    MatButtonModule,
    MatInputModule,
    MatFormFieldModule,
    MatSelectModule,
    MatRadioModule,
    MatTooltipModule
  ]
})
export class NodeModule { }
