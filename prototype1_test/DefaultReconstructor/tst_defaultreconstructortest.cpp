#include <QString>
#include <QtTest>

#include <cmath>
#include <cstdint>
#include <iostream>
#include <memory>
#include <vector>

#include <opencv2/core/core.hpp>

#include "configuration.h"
#include "defaultreconstructor.h"

using cv::Vec3d;
using std::vector;


class DefaultReconstructorTest : public QObject
{
	Q_OBJECT

private Q_SLOTS:
	void initTestCase();
	void cleanupTestCase();
	void testCase1_data();
	void testCase1();

private:
	std::unique_ptr<DefaultReconstructor> reconstructor = nullptr;
};


void DefaultReconstructorTest::initTestCase()
{
	Configuration::init(0, nullptr);
	reconstructor = std::make_unique<DefaultReconstructor>();
}

void DefaultReconstructorTest::cleanupTestCase()
{
	reconstructor = nullptr;
}

void DefaultReconstructorTest::testCase1_data()
{
	QTest::addColumn<double>("fov");
	QTest::addColumn<double>("pitch");
	QTest::addColumn<double>("posX");
	QTest::addColumn<double>("posY");
	QTest::addColumn<double>("posZ");
	QTest::addColumn<double>("skew");
	QTest::addColumn<int>("lineSizeX");
	QTest::addColumn<int>("lineSizeY");
	QTest::addColumn<int>("sampleX");
	QTest::addColumn<int>("sampleY");
	QTest::addColumn<double>("resultX");
	QTest::addColumn<double>("resultY");
	QTest::addColumn<double>("resultZ");

	double fov = M_PI_2;
	double pitch = M_PI_4;
	double posX = -1.0, posY = 0.0, posZ = 0.0;
	double skew = 0.;
	int lineSizeX = 3, lineSizeY = 3;
	int sampleX = 1, sampleY = 1;
	double resultX = 0.0, resultY = 0.0, resultZ = -1.0;

	QTest::newRow("Center (45° laser pitch, vertical)")
			<< fov << pitch << posX << posY << posZ << skew
			<< lineSizeX << lineSizeY << sampleX << sampleY
			<< resultX << resultY << resultZ;

	sampleX = 0;
	sampleY = 0;
	resultX = -0.5;
	resultY = 0.5;
	resultZ = -0.5;

	QTest::newRow("Top Left (45° laser pitch, vertical)")
			<< fov << pitch << posX << posY << posZ << skew
			<< lineSizeX << lineSizeY << sampleX << sampleY
			<< resultX << resultY << resultZ;

	posX = -2.0;
	posY = 0.0;
	posZ = 1.0;

	QTest::newRow("Top Left (45° laser pitch, vertical, laser behind)")
			<< fov << pitch << posX << posY << posZ << skew
			<< lineSizeX << lineSizeY << sampleX << sampleY
			<< resultX << resultY << resultZ;

	skew = M_PI_2;
	posX = 0.0;
	posY = 1.0;
	posZ = 0.0;
	sampleX = 0;
	sampleY = 1;
	resultX = -1.0;
	resultY = 0.0;
	resultZ = -1.0;

	QTest::newRow("Center Left (45° laser pitch, horizontal)")
			<< fov << pitch << posX << posY << posZ << skew
			<< lineSizeX << lineSizeY << sampleX << sampleY
			<< resultX << resultY << resultZ;

	pitch = M_PI_4;
	posX = -cos(M_PI_4);
	posY = sin(M_PI_4);
	skew = M_PI_4;
	sampleX = 1;
	sampleY = 1;
	resultX = 0.0;
	resultY = 0.0;
	resultZ = -1.0;

	QTest::newRow("Center (45° laser pitch, 45° skew)")
			<< fov << pitch << posX << posY << posZ << skew
			<< lineSizeX << lineSizeY << sampleX << sampleY
			<< resultX << resultY << resultZ;

	sampleX = 0;
	sampleY = 2;
	resultX = -1.0;
	resultY = -1.0;
	resultZ = -1.0;

	QTest::newRow("Buttom Right (45° laser pitch, 45° skew)")
			<< fov << pitch << posX << posY << posZ << skew
			<< lineSizeX << lineSizeY << sampleX << sampleY
			<< resultX << resultY << resultZ;

	pitch = atan(cos(-M_PI_4) * tan(M_PI_4));
	posX = -1.0;
	posY = 0.0;
	posZ = 0.0;
	skew = -M_PI_4;
	sampleX = 1;
	sampleY = 1;
	resultX = 0.0;
	resultY = 0.0;
	resultZ = -1.0;

	QTest::newRow("Center (45° horizontal pitch, -45° skew)")
			<< fov << pitch << posX << posY << posZ << skew
			<< lineSizeX << lineSizeY << sampleX << sampleY
			<< resultX << resultY << resultZ;

	sampleX = 0.0;
	sampleY = 0.0;
	resultX = -1.0;
	resultY = 1.0;
	resultZ = -1.0;

	QTest::newRow("Top Left (45° horizontal pitch, -45° skew)")
			<< fov << pitch << posX << posY << posZ << skew
			<< lineSizeX << lineSizeY << sampleX << sampleY
			<< resultX << resultY << resultZ;
}

void DefaultReconstructorTest::testCase1()
{
	QFETCH(double, fov);
	QFETCH(double, pitch);
	QFETCH(double, posX);
	QFETCH(double, posY);
	QFETCH(double, posZ);
	QFETCH(double, skew);
	QFETCH(int, lineSizeX);
	QFETCH(int, lineSizeY);
	QFETCH(int, sampleX);
	QFETCH(int, sampleY);
	QFETCH(double, resultX);
	QFETCH(double, resultY);
	QFETCH(double, resultZ);

	DeviceConfiguration conf;
	Line line{lineSizeX, lineSizeY};
	vector<Vec3d> result;

	conf.fov = fov;
	conf.projectorPitch = pitch;
	conf.projectorPos = Vec3d{posX, posY, posZ};
	conf.projectorSkew = skew;
	line.addSample(sampleX, sampleY);
	result.emplace_back(resultX, resultY, resultZ);

	Reconstruction r;
	reconstructor->setDeviceConfiguration(conf);
	reconstructor->processLine(line, r);

	QVERIFY2(r.getPoints().size() == result.size(), "Wrong amount of points");
	for (std::size_t i = 0; i < result.size(); ++i) {
		QVERIFY2(cv::norm(r.getPoints()[i].pos - result[i]) < 0.000001, "Wrong point");
	}
}

QTEST_APPLESS_MAIN(DefaultReconstructorTest)

#include "tst_defaultreconstructortest.moc"
