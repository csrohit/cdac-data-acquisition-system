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

  findAll(): Observable<Array<INodeType>>{
    return this.http.get<INodeType[]>(this.BASE);
  }

  findById(id: INodeType['id']): Observable<INodeType>{
    return this.http.get<INodeType>(`${this.BASE}/${id}`);
  }


}
