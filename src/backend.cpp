#include "backend.h"

void Backend::install()
{
    addRepositories();

    //Code to install packages

    zypp::ZYpp::Ptr zypp_pointer = zypp::getZYpp();
    zypp_pointer->initializeTarget( "/" );
    zypp::ResPoolProxy selectablePool( zypp::ResPool::instance().proxy() );

    foreach( QString package, packages() ) {
        zypp::ui::Selectable::Ptr s = zypp::ui::Selectable::get( package.toStdString() );
        zypp::PoolItem p = s->highestAvailableVersionObj();
        s->setCandidate( p );
        p.status().setToBeInstalled( zypp::ResStatus::USER );
    }

    zypp::ResPool pool = zypp::ResPool::instance();
    bool resolved = false;
    resolved = pool.resolver().resolvePool();

    if( !resolved ) {
        std::cout << "Failed to Resolve Pool" << std::endl;
        m_errorCode = 2;
        return;
    } else {
        std::cout << "Resolved Pool" << std::endl;
        //Perform Instalation
        zypp::ZYppCommitPolicy policy;
        policy.restrictToMedia( 0 );
        policy.downloadMode( zypp::DownloadInHeaps );
        policy.syncPoolAfterCommit();
        zypp::ZYppCommitResult result = zypp_pointer->commit( policy );

        if( result.allDone() ) {
            std::cout << "Installation Succeeded" << std::endl;
        } else {
            std::cout << "Installation did not succeed" << std::cout;
            m_errorCode = 3;
        }
    }
}

Backend::Backend()
{
    m_manager = new zypp::RepoManager;
    m_ptr = new zypp::ZYpp::Ptr;
    m_keyRingManager = new zypp::KeyRingCallbacks;
    m_errorCode = 0;
}

void Backend::addRepositories()
{
    qDebug() << "add repos called";

    foreach( QUrl iter, repositories() ) {
        zypp::RepoInfo repoInfo;
        std::string url = iter.toString().toStdString();
        repoInfo.addBaseUrl( zypp::Url( url ) );
        repoInfo.setAlias( url );
        repoInfo.setGpgCheck( true );

        if( !exists( url ) ) {
            try {
                m_manager->addRepository( repoInfo );
            } catch( zypp::repo::RepoMetadataException e ) {
                m_errorCode = 4;
            }
            catch( zypp::repo::RepoException ) {
                m_errorCode = 5;
            }
        }
        else
            qDebug() << "Repository Exists";
        m_manager->buildCache( repoInfo );
        m_manager->loadFromCache( repoInfo );
    }
}

bool Backend::exists( std::string repo )
{
    std::cout << "Parameter is " << repo << std::endl;
    std::list< zypp::RepoInfo > repoList = std::list< zypp::RepoInfo >( m_manager->repoBegin(), m_manager->repoEnd() );

    for( std::list< zypp::RepoInfo >::iterator it = repoList.begin(); it != repoList.end(); it++ ){
        //std::cout <<std::endl<<"Repo URL is " << it->url().asString();
        if( repo.compare( it->url().asString() ) == 0 )
                return true;
    }

    return false;
}

void Backend::callBackendHelper()
{
    QString command( "xdg-su -u root -c \"/usr/sbin/oneclickhelper " );
    command.append( getFileName() );
    command.append( "\"" );
    qDebug() << system( command.toLocal8Bit().data() );
}

int Backend::errorCode()
{
    return m_errorCode;
}
