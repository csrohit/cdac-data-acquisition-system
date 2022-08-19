import { Component, OnInit } from '@angular/core';
import { FormBuilder, FormControl, FormGroup, Validators } from '@angular/forms';
import { ActivatedRoute, Router } from '@angular/router';
import { first, Observable, of, share, shareReplay, switchMap } from 'rxjs';
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
  selectedNodeId: INode['id'] = undefined;


  constructor(
    private route: ActivatedRoute,
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
    this.route
    .params
    .pipe(
      switchMap(params => {
        if(params && params['nodeId']){
          this.selectedNodeId = params['nodeId'];
          return this.nodeService.findById(params['nodeId'])
        }
        return of(null);
      })
    )
    .subscribe(res =>{
      if(res){
        this.nodeForm.patchValue({
          ...res,
          type: res.type?.id
        })
      }
    })
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
    if(this.selectedNodeId){
      this.nodeService.update(this.selectedNodeId,this.node).pipe(first()).subscribe(() => this.router.navigate(['node']));
    }else{
      this.nodeService.save(this.node).pipe(first()).subscribe(() => this.router.navigate(['node']));
    }
  }


  onCancelClick(): void{
    this.router.navigate(['node']);
  }

}
