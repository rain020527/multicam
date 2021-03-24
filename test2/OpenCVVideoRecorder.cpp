#include "OpenCVVideoRecorder.h"
#include <windows.h>
#define num_cam 2

static std::string file[num_cam];
static FILE* pfile[num_cam];
static std::queue<std::pair<double, int>> timequeue;
static errno_t err;
static bool flag = true;
QMutex fclose_lock;
static int count = 0;
void m_push(double a, int id){
	timequeue.push({ a, id });
}
BaseException::BaseException(const char*fun, const char* msg)
	{
		try { if (NULL != fun) { m_Function = QString(fun); } }
		catch (...) {}
		try {
			if (NULL != msg) { m_Message = QString(msg); }
		}
		catch (...) {}
	}
BaseException::~BaseException() throw() {}
const QString& BaseException::Function() const { return m_Function; }
const QString& BaseException::Message() const { return m_Message; }

VideoRecorderException::VideoRecorderException(const char* fun, const char*msg)
		: BaseException(fun, msg)
	{}
VideoRecorderException::~VideoRecorderException() throw() {}


VmbUint32_t OpenCVRecorder::maxQueueElements() const { return 3; }
	

	void OpenCVRecorder::run()
	{
		//		clock_t time[3000];
		//		int count = 0;
		while (!m_StopThread)
		{
			double timetmp;
			int id;
			FrameStorePtr tmp;			
			{
				// two class events unlock the queue
				// first if a frame arrives enqueueFrame wakes the condition
				// second if the thread is stopped we are woken up
				// the while loop is necessary because a condition can be woken up by the system
				QMutexLocker local_lock(&m_ClassLock);
				while (!m_StopThread && m_FrameQueue.empty())
				{
					m_FramesAvailable.wait(local_lock.mutex());
				}
				if (!m_StopThread)
				{

					tmp = m_FrameQueue.front();
					m_FrameQueue.pop_front();
					timetmp = timequeue.front().first;
					id = timequeue.front().second;
					std::cout << "value is : " << timequeue.front().first << "\n";
					std::cout << "id is : " << timequeue.front().second << "\n";
					timequeue.pop();
				}
			}// scope for the lock, from now one we don't need the class lock
			if (!m_StopThread)
			{
				convertImage(*tmp);
				m_VideoWriter << m_ConvertImage;
				std::cout << "err is :" << err << "\n";
				if (err == 0) {
					std::cout << "now write\n";
					
					fputs((std::to_string(timetmp)+"\n").c_str(), pfile[id]);//check
				}/*
								if (count < 3000) {
									time[count] = clock();
									count++;
								}
				*/
			}

		}
		/*		double avg = 0, maxtime = 0;
				for (int i = 0; i < 2999; i++) {

					std::cout << i << " frame diff is : " << difftime(time[i+1] , time[i])<< "\n";
					avg += difftime(time[i + 1] , time[i]);
					if (maxtime < difftime(time[i + 1], time[i])) {
						maxtime = difftime(time[i + 1], time[i]);
					}
				}
				std::cout << "avg is :" << avg / 3000 << "\n";
				std::cout << "maxtime is :" << maxtime << "\n";
		 */
	}

	bool OpenCVRecorder::convertImage(frame_store &frame)
	{
		VmbImage srcImage;
		VmbImage dstImage;
		srcImage.Size = sizeof(srcImage);
		dstImage.Size = sizeof(dstImage);
		VmbSetImageInfoFromPixelFormat(frame.pixelFormat(), frame.width(), frame.height(), &srcImage);
		VmbSetImageInfoFromPixelFormat(VmbPixelFormatBgr8, m_ConvertImage.cols, m_ConvertImage.rows, &dstImage);
		srcImage.Data = frame.data();
		dstImage.Data = m_ConvertImage.data;
		return VmbErrorSuccess == VmbImageTransform(&srcImage, &dstImage, NULL, 0);

	}

	OpenCVRecorder::OpenCVRecorder(const QString &fileName, VmbFloat_t fps, VmbUint32_t Width, VmbUint32_t Height)
		: m_StopThread(false)
#ifdef _MSC_VER // codec selection only supported by Windows
		//, m_VideoWriter(fileName.toStdString(), FOURCC_USER_SELECT, fps, cv::Size(Width, Height), true)
		, m_VideoWriter(fileName.toStdString(), FOURCC_MJPEG, fps, cv::Size(Width, Height), true)
#else
		, m_VideoWriter(fileName.toStdString(), FOURCC_X264, fps, cv::Size(Width, Height), true)
#endif
		, m_ConvertImage(Height, Width, CV_8UC3)
	{

		int id = fileName.toStdString().at(22)-'0';
		file[id] = fileName.toStdString();
		err = fopen_s(&pfile[id], (file[id] + ".txt").c_str(), "w");
		std::cout << "id is " << id << std::endl;
		std::cout << fileName.toStdString() << std::endl;

		if (!m_VideoWriter.isOpened())
		{
			throw VideoRecorderException(__FUNCTION__, "could not open recorder");
		}
	}
	OpenCVRecorder::~OpenCVRecorder() { 
		
	}
	void OpenCVRecorder::stopThread()
	{
		QMutexLocker local_lock(&m_ClassLock);
		m_StopThread = true;
		m_FrameQueue.clear();
		m_FramesAvailable.wakeOne();

		std::cout << "aaa\n";
		fclose_lock.lock();

		std::cout << "gg";
		if (count == 0) {
			_fcloseall();//***********************************************************
			count++;
		}

		std::cout << "ya";

		fclose_lock.unlock();
	}
	bool OpenCVRecorder::enqueueFrame(const AVT::VmbAPI::Frame &frame)
	{
		VmbUint32_t         Width;
		VmbUint32_t         Height;
		VmbUint32_t         BufferSize;
		VmbPixelFormatType  PixelFormat;
		const VmbUchar_t*   pBuffer(NULL);

		if (VmbErrorSuccess == frame.GetPixelFormat(PixelFormat)
			&& VmbErrorSuccess == frame.GetWidth(Width)
			&& VmbErrorSuccess == frame.GetHeight(Height)
			&& VmbErrorSuccess == frame.GetBufferSize(BufferSize)
			&& VmbErrorSuccess == frame.GetBuffer(pBuffer))
		{
			if (m_ConvertImage.cols == Width
				&& m_ConvertImage.rows == Height)
			{
				QMutexLocker local_lock(&m_ClassLock);
				FrameStorePtr pFrame;
				// in case we reached the maximum number of queued frames
				// take of the oldest and reuse it to store the newly arriving frame
				if (m_FrameQueue.size() >= static_cast<FrameQueue::size_type>(maxQueueElements()))
				{
					pFrame = m_FrameQueue.front();
					m_FrameQueue.pop_front();
					if (!pFrame->equal(Width, Height, PixelFormat))
					{
						pFrame.clear();
					}
				}
				if (pFrame.isNull())
				{
					pFrame = FrameStorePtr(new frame_store(pBuffer, BufferSize, Width, Height, PixelFormat));
				}
				else
				{
					pFrame->setData(pBuffer, BufferSize);
				}
				m_FrameQueue.push_back(pFrame);
				m_FramesAvailable.wakeOne();
				return true;
			}
		}
		return false;
	}
