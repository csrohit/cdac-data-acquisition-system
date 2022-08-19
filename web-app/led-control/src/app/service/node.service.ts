import { HttpClient } from '@angular/common/http';
import { Injectable } from '@angular/core';
import { Observable, ObservableLike } from 'rxjs';
import { IDevice, INode, INodeType } from '../model/interfaces';

@Injectable({
  providedIn: 'root'
})
export class NodeService {

  /**
   * Base route for node apis
   */
  private readonly BASE = 'api/node';

  constructor(
    private http: HttpClient
  ) { }

  /**
   * Fetch all nodes
   * @returns Array of nodes
   */
  findAll(): Observable<INode[]>{
    return this.http.get<INode[]>(this.BASE);
  }

  /**
   * Find node details by id
   * @param id Id of node
   * @returns node details
   */
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

  /**
   * Save node details in database
   * @param node Node details
   * @returns saved node
   */
  save(node: INode): Observable<INode>{
    return this.http.post<INode>(this.BASE, node);
  }

  /**
   * Update node details
   * @param id id of node
   * @param node node details
   * @returns updated node
   */
  update(id: INode['id'], node: INode): Observable<INode>{
    return this.http.put<INode>(`${this.BASE}/${id}`, node);
  }

  /**
   * Deletes a node
   * @param id id of node
   * @returns none
   */
  delete(id: INode['id']): Observable<void>{
    return this.http.delete<void>(`${this.BASE}/${id}`);
  }

}
