#ifndef CASING_H
#define CASING_H

#include <QQuickItem>
#include <QFontMetrics>
#include <QPixmap>

#include "Plug.h"

class CasingBackend;
class Corkboard;

class Casing : public QQuickItem
{
    Q_OBJECT

    //Geometry, color
    Q_PROPERTY(QSize casingSize READ casingSize WRITE setCasingSize NOTIFY casingSizeChanged)

    //Name. If shown.
    Q_PROPERTY(QString ideaName READ ideaName WRITE setIdeaName NOTIFY ideaNameChanged)
    Q_PROPERTY(QSize nameSize READ nameSize NOTIFY nameSizeChanged)

    // Properties required for the plugs.
    Q_PROPERTY(int inPlugBoxTop READ inPlugBoxTop WRITE setInPlugBoxTop NOTIFY inPlugBoxTopChanged)
    Q_PROPERTY(int outPlugBoxTop READ outPlugBoxTop WRITE setOutPlugBoxTop NOTIFY outPlugBoxTopChanged)
    Q_PROPERTY(int inLabelWidth READ inLabelWidth WRITE setInLabelWidth NOTIFY inLabelWidthChanged)
    Q_PROPERTY(int outLabelWidth READ outLabelWidth WRITE setOutLabelWidth NOTIFY outLabelWidthChanged)
    Q_PROPERTY(QStringList inPlugLabels READ inPlugLabels NOTIFY inPlugLabelsChanged)
    Q_PROPERTY(QList<QColor> inPlugLabelColors READ inPlugLabelColors NOTIFY inPlugLabelColorsChanged)
    Q_PROPERTY(QStringList outPlugLabels READ outPlugLabels NOTIFY outPlugLabelsChanged)
    Q_PROPERTY(QList<QColor> outPlugLabelColors READ outPlugLabelColors NOTIFY outPlugLabelColorsChanged)

    // Properties for the feedback box.
    Q_PROPERTY(QColor feedbackColor READ feedbackColor WRITE setFeedbackColor NOTIFY feedbackColorChanged)
    Q_PROPERTY(QString feedbackMessage READ feedbackMessage WRITE setFeedbackMessage NOTIFY feedbackMessageChanged)
    Q_PROPERTY(int feedbackBoxTop READ feedbackBoxTop WRITE setFeedbackBoxTop NOTIFY feedbackBoxTopChanged)
    Q_PROPERTY(QSize feedbackBoxSize READ feedbackBoxSize WRITE setFeedbackBoxSize NOTIFY feedbackBoxSizeChanged)

    // Properties which are shown to the user.
    Q_PROPERTY(bool selected READ selected WRITE setSelected NOTIFY selectedChanged)
    Q_PROPERTY(bool resizeable READ resizeable NOTIFY resizeableChanged)

    // Multi-document interface type things
    Q_PROPERTY(bool minimizable READ minimizable NOTIFY minimizableChanged)
    Q_PROPERTY(bool minimized READ minimized WRITE setMinimized NOTIFY minimizedChanged)
    Q_PROPERTY(bool hasExtraQML READ hasExtraQML WRITE setHasExtraQML NOTIFY hasExtraQMLChanged)

public:
    explicit Casing(QQuickItem *parent = nullptr);
    ~Casing() override;

    void finishSetup();
    void setupCaption();
    void setupInPlugs();
    void setupOutPlugs();
    void setupFeedbackBox();

    QJsonObject save() const;
    void load (QJsonObject const &json);

    Corkboard* corkboard() const;
    CasingBackend* casingBackend();

    void setCorkboard(Corkboard* corkboard);
    void setBackend(CasingBackend* backend);

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

    /// Recalculates the size of the box. Called when things change.
    void recalculateSize();
    QPointF plugScenePosition(int plug, PlugType plugType) const;
    ///Returns the plug which is closest to the mouse. Or -1 if there isn't one
    int mouseOverPlug(PlugType plugType, QPointF mousePoint, double scale = 1.0);

    bool checkHitResizer(QPointF point) const;
    void repositionQML() const;

public:
    QSize casingSize() const;

    QString ideaName() const;
    QSize nameSize() const;

    int inPlugBoxTop() const;
    int outPlugBoxTop() const;
    int inLabelWidth() const;
    int outLabelWidth() const;
    QStringList inPlugLabels() const;
    QList<QColor> inPlugLabelColors() const;
    QStringList outPlugLabels() const;
    QList<QColor> outPlugLabelColors() const;

    QColor feedbackColor() const;
    QString feedbackMessage() const;
    int feedbackBoxTop() const;
    QSize feedbackBoxSize() const;

    bool embeddedQML() const;

    bool selected() const;
    bool resizeable() const;

    bool minimizable() const;
    bool minimized() const;
    bool hasExtraQML() const;

    void setInPlugColor(int plug, QColor color);
    void setInPlugToDefaultColor(int plug);
    void setOutPlugColor(int plug, QColor color);
    QColor getOutPlugColor(int plug);

    void setInPlugLabelsColor(QList<QColor> inPlugLabelColors);
    void setOutPlugLabelsColor(QList<QColor> outPlugLabelColors);

    void setIdeaName(const QString &newIdeaName);

signals:
    void casingSizeChanged(QSize casingSize);

    void ideaNameChanged(QString ideaName);
    void nameSizeChanged(QSize nameSize);

    void inPlugBoxTopChanged(int inPlugBoxTop);
    void outPlugBoxTopChanged(int outPlugBoxTop);
    void inLabelWidthChanged(int inLabelWidth);
    void outLabelWidthChanged(int outLabelWidth);
    void inPlugLabelsChanged(QStringList inPlugLabels);
    void inPlugLabelColorsChanged(QList<QColor> inPlugLabelColors);
    void outPlugLabelsChanged(QStringList outPlugLabels);
    void outPlugLabelColorsChanged(QList<QColor> outPlugLabelColors);

    void feedbackColorChanged(QColor feedbackColor);
    void feedbackMessageChanged(QString feedbackMessage);
    void feedbackBoxTopChanged(int feedbackBoxTop);
    void feedbackBoxSizeChanged(QSize feedbackBoxSize);

    void selectedChanged(bool selected);
    void resizeableChanged(bool resizeable);

    void minimizableChanged(bool minimizable);
    void minimizedChanged(bool minimized);
    void hasExtraQMLChanged(bool hasExtraQML);

public slots:
    void setCasingSize(QSize casingSize);

    void setInPlugBoxTop(int inPlugBoxTop);
    void setOutPlugBoxTop(int outPlugBoxTop);
    void setInLabelWidth(int inLabelWidth);
    void setOutLabelWidth(int outLabelWidth);

    void setFeedbackColor(QColor feedbackColor);
    void setFeedbackMessage(QString feedbackMessage);
    void setFeedbackBoxTop(int feedbackBoxTop);
    void setFeedbackBoxSize(QSize feedbackBoxSize);

    void setSelected(bool selected);
    void minimizeButtonClicked();
    void setMinimized(bool minimized);
    void extraButtonClicked();
    void setHasExtraQML(bool hasExtraQML);

private:
    Corkboard* m_corkboard;
    CasingBackend* m_backend;

    QPointF initialClickPos;
    QPointF initialSize;

    bool m_moving;
    bool m_resizing;

    int m_outPlugs;
    int m_inPlugs;

    bool m_hasEmbeddedQML;
    bool m_hasMinimizedQML;

    int m_QMLBoxX;
    int m_QMLBoxY;
    int m_QMLBoxWidth;
    int m_QMLBoxHeight;

    QSize m_casingSize;

    QString m_ideaName;
    QSize m_nameSize;

    int m_inPlugBoxTop;
    int m_outPlugBoxTop;
    int m_inLabelWidth;
    int m_outLabelWidth;
    QStringList m_inPlugLabels;
    QStringList m_outPlugLabels;

    QColor m_feedbackColor;
    QString m_feedbackMessage;
    int m_feedbackBoxTop;
    QSize m_feedbackBoxSize;

    bool m_selected;
    bool m_resizeable;

    bool m_minimizable;
    bool m_minimized;
    bool m_minimizedSetDuringLoad; // This handles an edge case I did not see coming.
    bool m_changeNameOnMinimized;
    bool m_hasExtraQML;

    bool m_nameChanged;

    QList<QColor> m_inPlugLabelColors;
    QList<QColor> m_outPlugLabelColors;
};

#endif // CASING_H
