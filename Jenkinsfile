pipeline
{
    agent any

    stages
    {
        /*
        Clean up the working tree by recursively removing files that are not
        under version control, starting from the current directory.
        Parameters explained:
        -x means ignored files are also removed. This can, for example, be
        useful to remove all build products.
        */
        stage('Clean up git repo')
        {
            steps
            {
                sh "git clean -fdx"
            }
        }

        stage('git test')
        {
            agent any
            steps
            {
                echo "GIT_COMMIT '$GIT_COMMIT'"
                echo "GIT_BRANCH '$GIT_BRANCH'"
            }
        }

        // stage('run lint on repo') {
        //     steps {
        //         sh "sh build/performLint.sh"
        //     }
        // }
    }
}
