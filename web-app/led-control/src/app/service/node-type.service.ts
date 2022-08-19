import { HttpClient } from '@angular/common/http';
import { Injectable } from '@angular/core';
import { Observable} from 'rxjs'
import { INode, INodeType } from '../model/interfaces';

@Injectable({
  providedIn: 'root'
})
export class NodeTypeService {

  private readonly BASE = 'api/node-type';

  constructor(
    private http: HttpClient
  ) { }

  /**
   * Fetch all node types
   * @returns array of node types
   */
  findAll(): Observable<INodeType[]>{
    return this.http.get<INodeType[]>(this.BASE);
  }

  /**
   * Fetch node type by id
   * @param id id of node type
   * @returns Observalble of node type
   */
  findById(id: INodeType['id']): Observable<INodeType>{
    return this.http.get<INodeType>(`${this.BASE}/${id}`);
  }


}
