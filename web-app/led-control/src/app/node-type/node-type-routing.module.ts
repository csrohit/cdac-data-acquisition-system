import { NgModule } from '@angular/core';
import { RouterModule, Routes } from '@angular/router';
import { NodeTypeComponent } from './node-type.component';

const routes: Routes = [{ path: '', component: NodeTypeComponent }];

@NgModule({
  imports: [RouterModule.forChild(routes)],
  exports: [RouterModule]
})
export class NodeTypeRoutingModule { }
