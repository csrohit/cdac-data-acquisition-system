import { TestBed } from '@angular/core/testing';

import { NodeTypeService } from './node-type.service';

describe('NodeTypeService', () => {
  let service: NodeTypeService;

  beforeEach(() => {
    TestBed.configureTestingModule({});
    service = TestBed.inject(NodeTypeService);
  });

  it('should be created', () => {
    expect(service).toBeTruthy();
  });
});
