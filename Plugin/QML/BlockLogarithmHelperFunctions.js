function setupActionPicture(N)
{
    clearActionPicture();

    let boxComponent;
    let boxObject;

    let boxSize = 20;

    let i;

    if(N < 3)
    {
        let i;
        //Setup top of action picture.
        for(i = 0; i < Math.floor(N); i++)
        {
            boxComponent = Qt.createComponent("BlockLogarithmDisplayBox.qml");
            boxObject = boxComponent.createObject(topRectanglesContainer, {
                                                      borderWidth: boxSize / 10,
                                                      x: (30 - 10*N) + boxSize*i,
                                                      y: 0,
                                                      width: boxSize, height: boxSize})
        }
        if(N%1 !== 0)
        {
            boxComponent = Qt.createComponent("BlockLogarithmDisplayBox.qml");
            boxObject = boxComponent.createObject(topRectanglesContainer, {
                                                      borderWidth: boxSize / 10,
                                                      x: (30 - 10*N) + boxSize*Math.floor(N),
                                                      y: 0,
                                                      width: boxSize*(N%1), height: boxSize})
        }
        //Setup middle action picture.
        boxComponent = Qt.createComponent("BlockLogarithmDisplayBox.qml");
        boxObject = boxComponent.createObject(midRectanglesContainer, {
                                                  borderWidth: boxSize / 10,
                                                  x: (30 - 10*N),
                                                  y: 0,
                                                  width: boxSize, height: boxSize})
        for(i = 1; i < Math.floor(N); i++)
        {
            boxComponent = Qt.createComponent("BlockLogarithmDisplayBox.qml");
            boxObject = boxComponent.createObject(midRectanglesContainer, {
                                                      borderWidth: boxSize / 10,
                                                      x: (30 - 10*N) + boxSize*i,
                                                      y: 0,
                                                      width: boxSize, height: boxSize})
            boxObject.setPartiallyFaded();
        }
        if(N%1 !== 0)
        {
            boxComponent = Qt.createComponent("BlockLogarithmDisplayBox.qml");
            boxObject = boxComponent.createObject(midRectanglesContainer, {
                                                      borderWidth: boxSize / 10,
                                                      x: (30 - 10*N) + boxSize*Math.floor(N),
                                                      y: 0,
                                                      width: boxSize*(N%1), height: boxSize})
            boxObject.setPartiallyFaded();
        }
    }
    else if (N <= 10)
    {

        boxSize = 10;
        //Setup top of action picture.
        for(i = 0; i < N; i++)
        {
            boxComponent = Qt.createComponent("BlockLogarithmDisplayBox.qml");
            boxObject = boxComponent.createObject(topRectanglesContainer, {
                                                      borderWidth: boxSize / 10,
                                                      x: 7 + boxSize * (i % 5),
                                                      y: 10 * Math.floor(i / 5.0),
                                                      width: boxSize, height: boxSize})
        }
        //Setup middle action picture.
        boxComponent = Qt.createComponent("BlockLogarithmDisplayBox.qml");
        boxObject = boxComponent.createObject(midRectanglesContainer, {
                                                  borderWidth: boxSize / 10,
                                                  x: 7,
                                                  y: 0,
                                                  width: boxSize, height: boxSize})
        for(i = 1; i < N; i++)
        {
            boxComponent = Qt.createComponent("BlockLogarithmDisplayBox.qml");
            boxObject = boxComponent.createObject(midRectanglesContainer, {
                                                      borderWidth: boxSize / 10,
                                                      x: 7 + boxSize * (i % 5),
                                                      y: 10 * Math.floor(i / 5.0),
                                                      width: boxSize, height: boxSize})
            boxObject.setPartiallyFaded();
        }
    }

    //Setup final box. Same most cases.
    boxComponent = Qt.createComponent("BlockLogarithmDisplayBox.qml");
    boxObject = boxComponent.createObject(botRectanglesContainer, {
                                              borderWidth: boxSize / 10,
                                              x: 30 - boxSize/2,
                                              y: 0 + (20 - boxSize) / 2,
                                              width: boxSize, height: boxSize})
}

function clearActionPicture()
{
    let i;
    for(i = 0; i < topRectanglesContainer.children.length; i++)
        topRectanglesContainer.children[i].destroy();
    for(i = 0; i < midRectanglesContainer.children.length; i++)
        midRectanglesContainer.children[i].destroy();
    for(i = 0; i < botRectanglesContainer.children.length; i++)
        botRectanglesContainer.children[i].destroy();
}
