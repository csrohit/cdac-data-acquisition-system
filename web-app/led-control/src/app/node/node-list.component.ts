import { Component, OnInit } from '@angular/core';
import { Router } from '@angular/router';
import { first, Observable } from 'rxjs';
import { INode } from '../model/interfaces';
import { NodeService } from '../service/node.service';

@Component({
  selector: 'app-node',
  templateUrl: './node-list.component.html',
  styleUrls: ['./node-list.component.scss']
})
export class NodeListComponent implements OnInit {

  nodes$!: Observable<INode[]>;

  constructor(
    private nodeService: NodeService,
    private router: Router
  ) {
  }

  ngOnInit(): void {

    this.nodes$ = this.nodeService
                      .findAll();
  }


  /**
   * Event handler for view devices button click
   * - display all the devices connected to clicked node
   * @param id id clicked node
   */
  onViewDevicesClick(id: INode['id']): void{
    this.router.navigate(['node', id, 'devices'])
  }

  /**
   * Event handler for add not button click
   * - navigate to add node component
   */
  onAddNodeClick(): void{
    this.router.navigate(['node', 'add'])
  }

  /**
   * Event handler for edit button click
   * @param id id of clicked node
   * - navigate to add node component in edit mode
   */
  onEditNodeClick(id: INode['id']): void{
    this.router.navigate(['node', id, 'edit']);
  }

  /**
   * Event handler for delete button click
   * - delete clicked node and reload nodes
   * @param id id of clicked node
   */
  onDeleteNodeClick(id: INode['id']): void{
    this.nodeService.delete(id).pipe(first()).subscribe(() => this.ngOnInit());
  }

}
