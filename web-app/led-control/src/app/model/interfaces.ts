export interface IIdLabelDescription{
    id: number;
    label: string;
    description: string
}


export interface INameLabelDescription{
    name: string;
    label: string;
    description: string
}

export interface INodeType extends IIdLabelDescription{
    
}


export interface INode extends IIdLabelDescription{
    type: INodeType;
    mqTopic: string;
}

export interface ICommand extends INameLabelDescription {
    hwValue: number;
}


export interface IDeviceType extends IIdLabelDescription{
    type: IDeviceType;
    cmdList: Array<ICommand>;
    retCmdList: Array<ICommand>;
}

export interface IDevice extends IIdLabelDescription{
    type: IDeviceType;
    node: INode;
    hwId: number;
}