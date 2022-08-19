import { HttpClient } from '@angular/common/http';
import { defineInjectable, Injectable } from '@angular/core';
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

  /**
   * Fetch all saved devices
   * @returns Array of devices
   */
  findAll(): Observable<IDevice[]>{
    return this.http.get<IDevice[]>(this.BASE);
  }

  /**
   * Update device details
   * @param id id of the device
   * @param device device details to be updated
   * @returns updated device
   */
  update(id: IDevice['id'], device: IDevice): Observable<IDevice>{
    return this.http.put<IDevice>(`${this.BASE}/${id}`, device);
  }

  /**
   * Delete device
   * @param id id of the device
   * @returns none
   */
  delete(id: IDevice['id']): Observable<void>{
    return this.http.delete<void>(`${this.BASE}/${id}`)
  }

}
