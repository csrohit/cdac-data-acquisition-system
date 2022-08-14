import { HttpClient } from '@angular/common/http';
import { Injectable } from '@angular/core';
import { Observable, ObservableLike } from 'rxjs';
import { IDevice, INode, INodeType } from '../model/interfaces';

@Injectable({
  providedIn: 'root'
})
export class NodeService {

  private readonly BASE = 'api/node';

  constructor(
    private http: HttpClient
  ) { }

  findAll(): Observable<INode[]>{
    return this.http.get<INode[]>(this.BASE);
  }

  findById(id: INode['id']): Observable<INode>{
    return this.http.get<INode>(`${this.BASE}/${id}`);
  }

  /**
   * Find all devices attached to node
   * @param id id of node
   * @returns array of devices attached to node
   */
  findDevices(id: INode['id']): Observable<IDevice[]>{
    return this.http.get<IDevice[]>(`${this.BASE}/${id}/devices`);
  }

}
