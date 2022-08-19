import { HttpClient } from '@angular/common/http';
import { Injectable } from '@angular/core';
import { Observable } from 'rxjs';
import { IDeviceType } from '../model/interfaces';

@Injectable({
  providedIn: 'root'
})
export class DeviceTypeService {

  private readonly BASE = 'api/device-type';

  constructor(
    private http: HttpClient
  ) { }

  /**
   * Fetch all saved device-types
   * @returns Array of device-types
   */
  findAll(): Observable<IDeviceType[]>{
    return this.http.get<IDeviceType[]>(this.BASE);
  }

  /**
   * fetch device-type details
   * @param id Id of device-type
   * @returns device-type details
   */
  findById(id: IDeviceType['id']): Observable<IDeviceType>{
    return this.http.get<IDeviceType>(`${this.BASE}/${id}`);
  }

  /**
   * Create device-type details
   * @param device-type device-type details
   * @returns saved device-type
   */
  save(deviceType: IDeviceType): Observable<IDeviceType>{
    return this.http.post<IDeviceType>(this.BASE, deviceType);
  }

  /**
   * Update device-type details
   * @param id id of the device-type
   * @param device-type device-type details to be updated
   * @returns updated device-type
   */
  update(id: IDeviceType['id'], deviceType: IDeviceType): Observable<IDeviceType>{
    return this.http.put<IDeviceType>(`${this.BASE}/${id}`, deviceType);
  }

  /**
   * Delete device-type
   * @param id id of the device-type
   * @returns none
   */
  delete(id: IDeviceType['id']): Observable<void>{
    return this.http.delete<void>(`${this.BASE}/${id}`)
  }

}
