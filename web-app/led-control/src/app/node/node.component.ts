import { Component, OnInit } from '@angular/core';
import { Router } from '@angular/router';
import { Observable } from 'rxjs';
import { INode } from '../model/interfaces';
import { NodeService } from '../service/node.service';

@Component({
  selector: 'app-node',
  templateUrl: './node.component.html',
  styleUrls: ['./node.component.scss']
})
export class NodeComponent implements OnInit {

  nodes$: Observable<INode[]>;

  constructor(
    private nodeService: NodeService,
    private router: Router
  ) {
    this.nodes$ = this.nodeService
                      .findAll();
  }

  ngOnInit(): void {
  }


  onViewDevicesClick(id: INode['id']): void{
    this.router.navigate(['node', id, 'devices'])
  }


}
