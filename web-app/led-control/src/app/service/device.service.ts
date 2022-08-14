import { HttpClient } from '@angular/common/http';
import { Injectable } from '@angular/core';
import { Observable } from 'rxjs';
import { INode, IDevice, ICommand } from '../model/interfaces';

@Injectable({
  providedIn: 'root'
})
export class DeviceService {

  private readonly BASE = 'api/device';

  constructor(
    private http: HttpClient
  ) { }

  /**
   * Run command on device
   * @param id id of device
   * @param name name of command
   * @returns mqtt message 
   */
  runCommand(id: IDevice['id'], name: ICommand['name']): Observable<any> {
    return this.http.put(`${this.BASE}/${id}/cmd`, { name });
  }

}
