import { NgModule } from '@angular/core';
import { RouterModule, Routes } from '@angular/router';
import { HomeComponent } from './home/home.component';

const routes: Routes = [
  { path: '', component: HomeComponent },
  { path: 'node-type', loadChildren: () => import('./node-type/node-type.module').then(m => m.NodeTypeModule) },
  { path: 'node', loadChildren: () => import('./node/node.module').then(m => m.NodeModule) },
  { path: 'device', loadChildren: () => import('./device/device.module').then(m => m.DeviceModule) },
  { path: 'device-type', loadChildren: () => import('./device-type/device-type.module').then(m => m.DeviceTypeModule) },
  { path: '**', redirectTo: '', pathMatch: 'full' }
];

@NgModule({
  imports: [RouterModule.forRoot(routes)],
  exports: [RouterModule]
})
export class AppRoutingModule { }
