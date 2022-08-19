import { Component, OnInit } from '@angular/core';
import { FormBuilder, FormControl, FormGroup, Validators } from '@angular/forms';
import { Router } from '@angular/router';
import { first, Observable, share, shareReplay } from 'rxjs';
import { INode, INodeType } from 'src/app/model/interfaces';
import { NodeTypeService } from 'src/app/service/node-type.service';
import { NodeService } from 'src/app/service/node.service';

@Component({
  selector: 'app-node-add',
  templateUrl: './node-add.component.html',
  styleUrls: ['./node-add.component.scss']
})
export class NodeAddComponent implements OnInit {

  nodeForm: FormGroup;
  node: INode
  nodeTypes$: Observable<INodeType[]>;


  constructor(
    private fb: FormBuilder,
    private nodeTypeService: NodeTypeService,
    private nodeService: NodeService,
    private router: Router
  ) {
    this.node = {
      description: '',
      label: '',
      mqTopic: '',
      type: {
        id: -1
      }
    };
    this.nodeForm = new FormGroup({
      label: new FormControl(this.node.label, Validators.required),
      description: new FormControl(this.node.label, Validators.required),
      mqTopic: new FormControl(this.node.label, Validators.required),
      type: new FormControl(this.node.type?.id, Validators.required)
    });

    this.nodeTypes$ = this.nodeTypeService.findAll().pipe(share())
  }

  ngOnInit(): void {
  }



  onSubmitClick(): void{
    this.node = {
      label: this.nodeForm.get('label')?.value,
      description: this.nodeForm.get('description')?.value,
      mqTopic: this.nodeForm.get('mqTopic')?.value,
      type: {
        id: this.nodeForm.get('type')?.value
      }
    }
    this.nodeService.save(this.node).pipe(first()).subscribe(() => this.router.navigate(['node']));
  }


  onCancelClick(): void{
    this.router.navigate(['node']);
  }

}
