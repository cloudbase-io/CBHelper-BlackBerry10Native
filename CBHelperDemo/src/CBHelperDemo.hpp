// Tabbed pane project template
#ifndef CBHelperDemo_HPP_
#define CBHelperDemo_HPP_

#include <QObject>
#include <QFile>
#include <QCryptographicHash>
#include <bb/cascades/Image>
#include <bb/cascades/ImageView>
#include <bb/cascades/Page>
#include <bb/cascades/WebView>
#include <bb/cascades/AbstractPane>

#include "CBHelper.h"
#include "CBSerializable.h"
#include "DataCommands/CBDataAggregationCommandProject.h"
#include "DataCommands/CBDataAggregationCommandGroup.h"
#include "DataCommands/CBDataAggregationCommandUnwind.h"

namespace bb { namespace cascades { class Application; }}

class TestData : public Cloudbase::CBSerializable {
public:
	TestData(std::string firstName, std::string lastName, std::string post) :
			first_name_(firstName), last_name_(lastName), post_(post) {}


	std::string serialize() {
		std::string out = "[ {";
		out += "\"first_name\" : \"" + first_name_ + "\", ";
		out += "\"last_name\" : \"" + last_name_ + "\", ";
		out += "\"post_\" : \"" + post_ + "\" } ]";

		return out;
	}
private:
	std::string first_name_;
	std::string last_name_;
	std::string post_;
};

/*!
 * @brief Application pane object
 *
 *Use this object to create and init app UI, to create context objects, to register the new meta types etc.
 */
class CBHelperDemo : public QObject, Cloudbase::CBHelperResponder
{
    Q_OBJECT
public:
    CBHelperDemo(bb::cascades::Application *app);
    virtual ~CBHelperDemo() {}

    Q_INVOKABLE void saveButtonClicked(QString appCode, QString appSecret, QString appPwd);
    Q_INVOKABLE void logButtonClicked(QString logMessage);
    Q_INVOKABLE void insertButtonClicked();
    Q_INVOKABLE void insertFileButtonClicked(QString filePath);
    Q_INVOKABLE void searchButtonClicked();
    Q_INVOKABLE void searchAggregateButtonClicked();
    Q_INVOKABLE void downloadFileButtonClicked(QString fileId);
    Q_INVOKABLE void execFunctionButtonClicked(QString functionCode);
    Q_INVOKABLE void execAppletButtonClicked();
    Q_INVOKABLE void payPalButtonClicked();

    virtual void parseResponse(Cloudbase::CBHelperResponseInfo resp);
private:
    Cloudbase::CBHelper* helper;
    bb::cascades::AbstractPane *root;

    TestData* doc;
    Cloudbase::CBHelperAttachment att;
    std::vector<Cloudbase::CBHelperAttachment> atts;
    Cloudbase::CBHelperSearchCondition *cond;
    Cloudbase::CBHelperDataCommandList *commandList;
    Cloudbase::CBDataAggregationCommandProject *project;
    Cloudbase::CBDataAggregationCommandGroup *group;
    bb::cascades::WebView *payPalWebView;
public Q_SLOTS:
	void monitorPayPalTransaction(bb::cascades::WebNavigationRequest *request);
};

#endif /* CBHelperDemo_HPP_ */
